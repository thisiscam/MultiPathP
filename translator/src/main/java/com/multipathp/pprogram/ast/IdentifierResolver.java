package com.multipathp.pprogram.ast;


import com.multipathp.pprogram.ErrorReporter;
import com.multipathp.pprogram.types.PType;

import java.util.ArrayList;

/**
 * Created by jianqiaoyang on 6/2/17.
 */
public class IdentifierResolver extends SimpleScopedPASTVisitor<Void> {
    private boolean isGlobalFunction;
    @Override
    public Void visit(PFunction pFunction) {
        isGlobalFunction = program.getGlobalFunctionDecls().get(pFunction.getName()) == pFunction;
        return super.visit(pFunction);
    }

    @Override
    public Void visit(CallExp callExp) {
        if(!isGlobalFunction) {
            if(machine.getFunDecls().containsKey(callExp.getFunctionName())) {
                PFunction callee = machine.getFunDecls().get(callExp.getFunctionName());
                callExp.setCallee(callee);
            }
        }
        if(program.getGlobalFunctionDecls().containsKey(callExp.getFunctionName())) {
            PFunction callee = program.getGlobalFunctionDecls().get(callExp.getFunctionName());
            callExp.setCallee(callee);
        }
        return super.visit(callExp);
    }

    @Override
    public Void visit(IdExp idExp) {
        String idName = idExp.getName();
        if(function.getLocalDecls().containsKey(idName)) {
            idExp.setExpressionType(function.getLocalDecls().get(idName));
        } else if(function.getParams().containsKey(idName)) {
            idExp.setExpressionType(function.getParams().get(idName));
        } else if(!isGlobalFunction && machine.getVarDecls().containsKey(idName)) {
            idExp.setExpressionType(machine.getVarDecls().get(idName));
        } else if (program.getEventDecls().containsKey(idName)) {
            idExp.setExpressionType(PType.EVENT);
        } else {
            //TODO: better error reporting mechanism
            System.out.printf("Cannot find identifier %s", idName);
            System.exit(-1);
        }
        return super.visit(idExp);
    }
}
