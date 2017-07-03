package com.multipathp.pprogram;
import static org.junit.Assert.*;

import com.multipathp.pprogram.ast.PProgram;
import com.multipathp.translator.basic_cpp.BasicCppTranslator;
import org.junit.Test;

import java.io.IOException;

public class ParseTreeSetParserTest {

    @Test
    public void testSingleFile() {
        String input = getClass().getResource("include/main.p").getFile();
        ParseTreeSetParser parser = new ParseTreeSetParser();
        ParseTreeSetParser.ParseTreeSet set = parser.parseFile(input);
        assertEquals(set.getParsedFilePaths().size(), 3);
        ParseTreeToPAST converter = new ParseTreeToPAST(set);
        PProgram program = converter.getProgram();
        assertEquals(program.getEventDecls().size(), 2);
        assertEquals(program.getTypedefs().size(), 1);
    }

    @Test
    public void testParsePingPong() throws IOException {
        String input = getClass().getResource("demo.p").getFile();
        ParseTreeSetParser parser = new ParseTreeSetParser();
        ParseTreeSetParser.ParseTreeSet set = parser.parseFile(input);
        assertEquals(set.getParsedFilePaths().size(), 1);
        ParseTreeToPAST converter = new ParseTreeToPAST(set);
        PProgram program = converter.getProgram();
        BasicCppTranslator translator = new BasicCppTranslator("pingpong", program);
        translator.translate("test_dir");
    }
}
