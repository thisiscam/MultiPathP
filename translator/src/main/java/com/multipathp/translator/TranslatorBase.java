package com.multipathp.translator;

import com.multipathp.pprogram.ast.PProgram;
import org.stringtemplate.v4.STGroupFile;

import java.io.IOException;
import java.nio.file.Paths;


public abstract class TranslatorBase {
    protected final PProgram program;
    protected final String projectName;

    public TranslatorBase(String projectName, PProgram program) {
        this.projectName = projectName;
        this.program = program;
    }

    protected STGroupFile getSTGroupFile(String path) {
        String root = getClass().getPackage().getName().replace('.', '/');
        return new STGroupFile(Paths.get(root, path).toString());
    }

    public abstract void translate(String outDir) throws IOException;
}
