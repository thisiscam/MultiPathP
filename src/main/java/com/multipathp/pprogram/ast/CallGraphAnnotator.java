package com.multipathp.pprogram.ast;

import java.util.*;

/**
 * Created by jianqiaoyang on 6/2/17.
 */
public class CallGraphAnnotator extends SimpleScopedPASTVisitor<Void> {
    class CanRaiseEventAnnotation extends PASTAnnotation {
        private boolean canRaiseEvent;

        boolean getCanRaiseEvent() {
            return canRaiseEvent;
        }

        public CanRaiseEventAnnotation(boolean canRaiseEvent) {
            this.canRaiseEvent = canRaiseEvent;
        }
    }

    private Queue<PFunction> canRaiseEventFunctions = new LinkedList<>();
    private Map<PFunction, Collection<PFunction>> callGraph = new HashMap<>();

    @Override
    public Void visit(PProgram pProgram) {
        super.visit(pProgram);
        while (canRaiseEventFunctions.size() > 0) {
            PFunction f = canRaiseEventFunctions.remove();
            CanRaiseEventAnnotation canRaiseEventAnnotation = f.getDecoration(CanRaiseEventAnnotation.class);
            if (!canRaiseEventAnnotation.canRaiseEvent) {
                canRaiseEventAnnotation.canRaiseEvent = true;
                if(callGraph.containsKey(f)) {
                    canRaiseEventFunctions.addAll(callGraph.get(f));
                }
            }
        }
        return null;
    }

    @Override
    public Void visit(PFunction pFunction) {
        pFunction.decorate(new CanRaiseEventAnnotation(false));
        return super.visit(pFunction);
    }

    @Override
    public Void visit(CallExp callExp) {
        callGraph.putIfAbsent(callExp.getCallee(), new ArrayList<>());
        callGraph.computeIfPresent(callExp.getCallee(), (callee, callers) -> {
            callers.add(function);
            return callers;
        });
        return super.visit(callExp);
    }

    @Override
    public Void visit(RaiseStmt raiseStmt) {
        canRaiseEventFunctions.add(function);
        return super.visit(raiseStmt);
    }

    @Override
    public Void visit(PopStmt popStmt) {
        canRaiseEventFunctions.add(function);
        return super.visit(popStmt);
    }
}
