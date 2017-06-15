package com.multipathp.pprogram.ast;


import com.multipathp.pprogram.types.PType;

/**
 * Created by jianqiaoyang on 6/2/17.
 */
public class IdentifierResolver extends SimpleScopedPASTVisitor<Void> {
    private boolean isGlobalFunction;

    private PFunction getCallee(String name) {
        if(name == null) {
            return null;
        }
        if(!isGlobalFunction) {
            if(machine.getFunDecls().containsKey(name)) {
                return machine.getFunDecls().get(name);

            }
        }
        if(program.getGlobalFunctionDecls().containsKey(name)) {
            return program.getGlobalFunctionDecls().get(name);
        }
        //TODO: better error reporting mechanism
        System.out.printf("Cannot find identifier %s", name);
        System.exit(-1);
        return null;
    }

    @Override
    public Void visit(PMachineState pMachineState) {
        pMachineState.setEntryFunction(getCallee(pMachineState.getEntryFunctionName()));
        pMachineState.setExitFunction(getCallee(pMachineState.getExitFunctionName()));
        return super.visit(pMachineState);
    }

    @Override
    public Void visit(PTransition pTransition) {
        pTransition.setFunction(getCallee(pTransition.getFunctionName()));
        return super.visit(pTransition);
    }

    @Override
    public Void visit(PFunction pFunction) {
        isGlobalFunction = program.getGlobalFunctionDecls().get(pFunction.getName()) == pFunction;
        return super.visit(pFunction);
    }

    @Override
    public Void visit(CallExp callExp) {
        callExp.setCallee(getCallee(callExp.getFunctionName()));
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
