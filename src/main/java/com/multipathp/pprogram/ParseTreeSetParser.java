package com.multipathp.pprogram;

import com.multipathp.pparser.pBaseVisitor;
import com.multipathp.pparser.pLexer;
import com.multipathp.pparser.pParser;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.Interval;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeProperty;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;
import org.antlr.v4.runtime.tree.pattern.ParseTreeMatch;
import org.antlr.v4.runtime.tree.pattern.ParseTreePattern;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;

public class ParseTreeSetParser {
    public static final pParser P_PARSER;

    static {
        CommonTokenStream tokenStream = new CommonTokenStream(new pLexer((CharStream) null));
        P_PARSER = new pParser(tokenStream);
    }

    private Collection<String> includeSearchPaths = new ArrayList<String>();
    private ParseTreePattern includePattern = P_PARSER.compileParseTreePattern("include <includeFilePath:STR>", pParser.RULE_include_decl);

    public ParseTreeSetParser() {
        this(Collections.emptyList());
    }

    public ParseTreeSetParser(Collection<String> includeSearchPaths) {
        super();
        this.includeSearchPaths.addAll(includeSearchPaths);
    }

    public ParseTreeSet parseFile(String filePath) {
        ParseTreeSet set = new ParseTreeSet();
        HashSet<String> parsedPaths = new HashSet<String>();
        pParser.ProgramContext program = parseFileHelper(filePath, set, parsedPaths, null);
        if(program == null) {
            return null;
        }
        return set;
    }

    private pParser.ProgramContext parseFileHelper(String filePath, ParseTreeSet set, HashSet<String> parsedPaths, pParser.Include_declContext includeDecl) {
        try {
            CharStream inputStream = CharStreams.fromFileName(filePath);
            CommonTokenStream tokenStream = new CommonTokenStream(new pLexer(inputStream));
            pParser parser = new pParser(tokenStream);
            pParser.ProgramContext tree = parser.program();
            if (parser.getNumberOfSyntaxErrors() != 0) {
                return null;
            }
            if(includeDecl != null) {
                set.addAST(includeDecl, tree, filePath);
            } else {
                set.setEntry(tree, filePath);
            }
            parsedPaths.add(filePath);
            includeFileHelper(filePath, set, parsedPaths, tree);
            return tree;
        } catch (IOException ioe) {
            ErrorReporter.error(ioe.getMessage());
        }
        return null;
    }

    private void includeFileHelper(String filePath, ParseTreeSet set, HashSet<String> parsedPaths, ParseTree tree) {
        for (ParseTreeMatch match : includePattern.findAll(tree, "//*")) {
            pParser.Include_declContext includeDecl = (pParser.Include_declContext) match.getTree();
            String includeFilePath = match.get("includeFilePath").getText();
            includeFilePath = includeFilePath.substring(1, includeFilePath.length() - 1); // remove double quotes
            ArrayList<String> allSearchPaths = new ArrayList<String>(includeSearchPaths);
            allSearchPaths.add(Paths.get(filePath).getParent().toString());
            boolean found = false;
            for (String includeSearchPath : allSearchPaths) {
                Path potentialPath = Paths.get(includeSearchPath, includeFilePath);
                if (Files.exists(potentialPath)) {
                    if (parsedPaths.contains(potentialPath.toUri().getPath())) {
                        ErrorReporter.warn("cyclic include", includeDecl, set);
                        return;
                    }
                    pParser.ProgramContext parsedTree = parseFileHelper(potentialPath.toUri().getPath(), set, parsedPaths, includeDecl);
                    if(parsedTree == null) {
                        ErrorReporter.error("Error parsing " + potentialPath.toString());
                    }
                    found = true;
                    break;
                }
            }
            if(!found) {
                ErrorReporter.error("include file not found", includeDecl, set);
            }
        }
    }

    public static class ParseTreeSet implements ParseTree {
        Collection<pParser.ProgramContext> parsedFiles;
        ParseTreeProperty<pParser.ProgramContext> includedFileProperty;
        ParseTreeProperty<String> filePathProperty;

        public ParseTreeSet() {
            parsedFiles = new ArrayList<>();
            includedFileProperty = new ParseTreeProperty<>();
            filePathProperty = new ParseTreeProperty<>();
        }

        public void addAST(pParser.Include_declContext includeDecl, pParser.ProgramContext ast, String filePath) {
            parsedFiles.add(ast);
            includedFileProperty.put(includeDecl, ast);
            filePathProperty.put(ast, filePath);
        }

        public pParser.ProgramContext getParsedFile(pParser.Include_declContext includeDecl) {
            return includedFileProperty.get(includeDecl);
        }

        public String getSourcePath(pParser.ProgramContext program) {
            return filePathProperty.get(program);
        }

        public String getSourceName(pParser.ProgramContext program) {
            String path = getSourcePath(program);
            return Paths.get(path).getFileName().toString();
        }

        public void setEntry(pParser.ProgramContext program, String path) {
            parsedFiles.add(program);
            includedFileProperty.put(null, program);
            filePathProperty.put(program, path);
        }

        public Collection<String> getParsedFilePaths() {
            return parsedFiles.stream().map((program) -> filePathProperty.get(program)).collect(Collectors.toList());
        }

        public pParser.ProgramContext getEntry() {
            return includedFileProperty.get(null);
        }

        @Override
        public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
            return getEntry().accept(visitor);
        }

        @Override
        public Interval getSourceInterval() {
            return getEntry().getSourceInterval();
        }

        @Override
        public ParseTree getChild(int i) {
            return getEntry().getChild(i);
        }

        @Override
        public int getChildCount() {
            return getEntry().getChildCount();
        }

        @Override
        public String toStringTree() {
            throw new UnsupportedOperationException();
        }

        @Override
        public ParseTree getParent() {
            throw new UnsupportedOperationException();
        }

        @Override
        public void setParent(RuleContext ruleContext) {
            throw new UnsupportedOperationException();
        }

        @Override
        public Object getPayload() {
            throw new UnsupportedOperationException();
        }

        @Override
        public String getText() {
            throw new UnsupportedOperationException();
        }

        @Override
        public String toStringTree(Parser parser) {
            throw new UnsupportedOperationException();
        }
    }

    public static class ASTSetVisitorBase<T> extends pBaseVisitor<T> {
        protected ParseTreeSet set;

        public ASTSetVisitorBase(ParseTreeSet set) {
            this.set = set;
        }

        public T visit() {
            return set.accept(this);
        }

        @Override
        public T visitInclude_decl(pParser.Include_declContext ctx) {
            pParser.ProgramContext program = set.getParsedFile(ctx);
            if (program != null) {
                return program.accept(this);
            }
            return null;
        }
    }
}