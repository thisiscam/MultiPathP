package com.multipathp.translator.basic_cpp;


import com.multipathp.pprogram.ast.PMachine;
import com.multipathp.pprogram.ast.PProgram;
import com.multipathp.translator.TranslatorBase;
import org.apache.commons.io.FileUtils;
import org.stringtemplate.v4.ST;
import org.stringtemplate.v4.STGroupFile;

import java.io.File;
import java.io.IOException;

public class BasicCppTranslator extends TranslatorBase {

    public BasicCppTranslator(PProgram program) {
        super(program);
    }

    @Override
    public void translate(String outDir) throws IOException {
        STGroupFile headerTemplateGroup = new STGroupFile(getClass().getResource("header.stg").getPath());
        ST headerTemplate = headerTemplateGroup.getInstanceOf("HeaderFile");
        headerTemplate.add("program", program);
        FileUtils.writeStringToFile(new File(outDir, "header.h"), headerTemplate.render());

        STGroupFile machineTemplateGroup = new STGroupFile(getClass().getResource("machine.stg").getPath());
        for (PMachine machine : program.getMachines()) {
            ST machineImplementationTemplate = machineTemplateGroup.getInstanceOf("MachineImplementationFile");
            machineImplementationTemplate.add("program", program);
            machineImplementationTemplate.add("machine", machine);
            String fileName = machineTemplateGroup.getInstanceOf("MachineImplementationFileName").add("machine", machine).render();
            File machineImplementationFile = new File(outDir, fileName);
            FileUtils.writeStringToFile(machineImplementationFile, machineImplementationTemplate.render());
        }
    }
}
