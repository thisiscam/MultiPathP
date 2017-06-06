package com.multipathp.translator;

import com.multipathp.pprogram.ast.PProgram;

import java.io.IOException;


public abstract class TranslatorBase {
    protected PProgram program;

    public TranslatorBase(PProgram program) {
        this.program = program;
    }

    public abstract void translate(String outDir) throws IOException;
}
