package com.multipathp.translator;


import com.beust.jcommander.JCommander;
import com.beust.jcommander.Parameter;
import com.beust.jcommander.ParameterException;
import com.multipathp.pprogram.ParseTreeSetParser;
import com.multipathp.pprogram.ParseTreeToPAST;
import com.multipathp.pprogram.ast.PProgram;
import com.multipathp.translator.basic_cpp.BasicCppTranslator;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by jianqiaoyang on 6/2/17.
 */
public class CommandLineTool {
    static class CommandLineOptions {
        @Parameter(names = {"-t", "--translator"}, description = "translator to use")
        String translator = BasicCppTranslator.class.getSimpleName();

        @Parameter(names = {"-I", "--include"}, description = "include search directories")
        List<String> includeDirectories = new ArrayList<>();

        @Parameter(names = {"-o", "--out-dir"}, description = "output directory")
        String outDir = "";

        @Parameter(names = {"--name"}, description = "name of the project, default to input file name")
        String projectName = null;

        @Parameter(names = {"-h", "--help"}, description = "displays this help message", help = true)
        boolean displayUsage;

        @Parameter(description = "input-file", required = true)
        String inputFile;
    }

    public static void main(String[] args) throws ClassNotFoundException, NoSuchMethodException, IllegalAccessException, InvocationTargetException, InstantiationException, IOException {
        CommandLineOptions options = new CommandLineOptions();
        JCommander cmdParser = JCommander.newBuilder().addObject(options).build();
        try {
            cmdParser.parse(args);
        } catch (ParameterException pe) {
            System.out.println(pe.getMessage());
            pe.usage();
            System.exit(-1);
        }
        if(options.displayUsage) {
            cmdParser.usage();
            System.exit(0);
        }
        if(options.projectName == null) {
            options.projectName = FilenameUtils.getBaseName(options.inputFile);
        }
        Constructor translatorConstructor = Class.forName(BasicCppTranslator.class.getPackage().getName() + "." + options.translator).getConstructor(String.class, PProgram.class);
        ParseTreeSetParser.ParseTreeSet set = new ParseTreeSetParser(options.includeDirectories).parseFile(options.inputFile);
        PProgram program = new ParseTreeToPAST(set).getProgram();
        TranslatorBase translator = (TranslatorBase) translatorConstructor.newInstance(options.projectName, program);
        translator.translate(options.outDir);
    }
}