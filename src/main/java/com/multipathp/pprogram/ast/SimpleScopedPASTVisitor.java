package com.multipathp.pprogram.ast;

/**
 * Created by jianqiaoyang on 6/1/17.
 */
public class SimpleScopedPASTVisitor<T> extends PASTVisitor<T> {
    protected PProgram program;
    protected PMachine machine;
    protected PMachineState state;
    protected PFunction function;

    @Override
    public T visit(PProgram pProgram) {
        program = pProgram;
        return super.visit(pProgram);
    }

    @Override
    public T visit(PMachine pMachine) {
        machine = pMachine;
        return super.visit(pMachine);
    }

    @Override
    public T visit(PMachineState pMachineState) {
        state = pMachineState;
        return super.visit(pMachineState);
    }

    @Override
    public T visit(PFunction pFunction) {
        function = pFunction;
        return super.visit(pFunction);
    }
}
