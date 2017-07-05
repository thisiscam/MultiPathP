package com.multipathp.pprogram;

import com.multipathp.pparser.pParser;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

public class ErrorReporter {
    private static String getSourceName(ParserRuleContext tree, ParseTreeSetParser.ParseTreeSet set) {
        while (tree.getParent() != null) {
            tree = tree.getParent();
        }
        return set.getSourceName((pParser.ProgramContext) tree);
    }

    static void error(String msg) {
        System.out.format(msg);
        System.exit(1);
    }

    static void error(String msg, ParserRuleContext ctx, ParseTreeSetParser.ParseTreeSet set) {
        Token tok = ctx.getStart();
        System.out.format("Error: %s in %s @[line: %d, col: %d]\n", msg, getSourceName(ctx, set), tok.getLine(), tok.getCharPositionInLine());
        System.exit(1);
    }

    static void warn(String msg, ParserRuleContext ctx, ParseTreeSetParser.ParseTreeSet set) {
        Token tok = ctx.getStart();
        System.out.format("Warning: %s in %s @[line: %d, col: %d]\n", msg, getSourceName(ctx, set), tok.getLine(), tok.getCharPositionInLine());
    }
}