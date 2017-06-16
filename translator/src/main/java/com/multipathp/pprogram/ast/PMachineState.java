package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.PEvent;
import org.inferred.freebuilder.FreeBuilder;

import javax.annotation.Nullable;
import java.util.*;

@FreeBuilder
public abstract class PMachineState extends PASTNode {
    public abstract String getName();
    public abstract boolean isStart();
    @Nullable
    public abstract String getEntryFunctionName();
    @Nullable
    public abstract String getExitFunctionName();
    public abstract Map<String, PEvent> getDeferedEvents();

    public abstract List<PTransition> getTransitions();
    public abstract Map<PEvent, PTransition> getTransitionsMap();

    public static PMachineState HALT_STATE = new PMachineHaltState();
    @Nullable
    private PFunction entryFunction;
    public PFunction getEntryFunction() {
        if(getEntryFunctionName() != null && entryFunction == null) {
            throw new IllegalStateException("entryFunction must be set first");
        }
        return entryFunction;
    }

    public void setEntryFunction(PFunction entryFunction) {
        if(this.entryFunction != null) {
            throw new IllegalStateException("entryFunction cannot be re-set");
        }
        this.entryFunction = entryFunction;
    }

    @Nullable
    private PFunction exitFunction;
    public PFunction getExitFunction() {
        if(getExitFunctionName() != null && exitFunction == null) {
            throw new IllegalStateException("exitFunction must be set first");
        }
        return exitFunction;
    }

    public void setExitFunction(PFunction exitFunction) {
        if(this.exitFunction != null) {
            throw new IllegalStateException("exitFunction cannot be re-set");
        }
        this.exitFunction = exitFunction;
    }


    @Override
    public int getChildrenCount() {
        return getTransitions().size();
    }

    @Override
    public PASTNode getChild(int i) {
        return getTransitions().get(i);
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends PMachineState_Builder {
        private Map<PEvent, PTransition.Builder> transitionBuilderMap;

        public Builder() {
            super();
            addTransitions(PTransition.HALT_TRANSITION);
            putTransitionsMap(PEvent.EVENT_HALT, PTransition.HALT_TRANSITION);
        }

        public void addTransition(String functionName, Collection<PEvent> eventList, String toStateName, boolean isPush) {
            for(PEvent e : eventList) {
                PTransition transition = new PTransition.Builder().setFunctionName(functionName).setOnE(e).setToStateName(toStateName).setPush(isPush).build();
                putTransitionsMap(e, transition);
                addTransitions(transition);
            }
        }

        public void addTransition(PFunction.Builder function, Collection<PEvent> eventList, String toStateName, boolean isPush) {
            for(PEvent e : eventList) {
                PTransition transition = new PTransition.Builder().setFunctionName(function.getName()).setOnE(e).setToStateName(toStateName).setPush(isPush).build();
                putTransitionsMap(e, transition);
                addTransitions(transition);
            }
        }
    }
}