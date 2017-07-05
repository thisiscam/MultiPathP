package com.multipathp.pprogram;
import static org.junit.Assert.*;

import com.multipathp.pprogram.ast.PProgram;
import com.multipathp.translator.basic_cpp.CppTranslator;
import org.junit.Test;

import java.io.IOException;

public class ParseTreeSetParserTest {

    @Test
    public void testSingleFile() {
        String input = getClass().getResource("include/main.p").getFile();
        ParseTreeSetParser parser = new ParseTreeSetParser();
        ParseTreeSetParser.ParseTreeSet set = parser.parseFile(input);
        assertEquals(3, set.getParsedFilePaths().size());
        ParseTreeToPAST converter = new ParseTreeToPAST(set);
        PProgram program = converter.getProgram();
        assertEquals(4, program.getEventDecls().size());
        assertEquals(1, program.getTypedefs().size());
    }

    @Test
    public void testParsePingPong() throws IOException {
        String input = getClass().getResource("two-phase-commit.p").getFile();
        ParseTreeSetParser parser = new ParseTreeSetParser();
        ParseTreeSetParser.ParseTreeSet set = parser.parseFile(input);
        assertEquals(1, set.getParsedFilePaths().size());
        ParseTreeToPAST converter = new ParseTreeToPAST(set);
        PProgram program = converter.getProgram();
        CppTranslator translator = new CppTranslator("pingpong", program);
        translator.translate("test_dir");
    }
}
