package com.multipathp.translator.basic_cpp;


import com.multipathp.pprogram.ast.*;
import com.multipathp.pprogram.types.PType;
import com.multipathp.pprogram.types.PTypeNamedTuple;
import com.multipathp.pprogram.types.PTypeTuple;
import com.multipathp.translator.TranslatorBase;
import org.apache.commons.io.FileUtils;
import org.stringtemplate.v4.ST;
import org.stringtemplate.v4.STGroupFile;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Executable;
import java.util.HashSet;
import java.util.Set;

public class BasicCppTranslator extends TranslatorBase {

    static class PTypeCollector extends PASTVisitor<Set<PType>> {
        private Set<PType> usedTypes = new HashSet<>();
        @Override
        protected Set<PType> visitChildren(PASTNode node) {
            if(node instanceof Exp) {
                PType type = ((Exp) node).getExpressionType();
                if(type instanceof PTypeNamedTuple) {
                    type = new PTypeTuple(((PTypeNamedTuple)type).getInnerTypes());
                }
                usedTypes.add(type);
            }
            super.visitChildren(node);
            return usedTypes;
        }
    }

    public BasicCppTranslator(String projectName, PProgram program) {
        super(projectName, program);
    }

    @Override
    public void translate(String outDir) throws IOException {
        Set<PType> usedTypes = program.accept(new PTypeCollector());
        STGroupFile headerTemplateGroup = getSTGroupFile("header.stg");
        ST headerTemplate = headerTemplateGroup.getInstanceOf("HeaderFile");
        headerTemplate.add("projectName", projectName);
        headerTemplate.add("program", program);
        headerTemplate.add("declTypes", usedTypes);
        FileUtils.writeStringToFile(new File(outDir, "header.h"), headerTemplate.render());

        STGroupFile machineTemplateGroup = getSTGroupFile("machine.stg");
        for (PMachine machine : program.getMachines()) {
            ST machineImplementationTemplate = machineTemplateGroup.getInstanceOf("MachineImplementationFile");
            machineImplementationTemplate.add("projectName", projectName);
            machineImplementationTemplate.add("program", program);
            machineImplementationTemplate.add("machine", machine);
            String fileName = machineTemplateGroup.getInstanceOf("MachineImplementationFileName").add("machine", machine).render();
            File machineImplementationFile = new File(outDir, fileName);
            FileUtils.writeStringToFile(machineImplementationFile, machineImplementationTemplate.render());
        }
    }
}
