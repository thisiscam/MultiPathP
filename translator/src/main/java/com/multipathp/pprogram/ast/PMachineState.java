package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.PEvent;
import org.inferred.freebuilder.FreeBuilder;

import java.util.*;

@FreeBuilder
public abstract class PMachineState extends PASTNode {
    public abstract String getName();
    public abstract boolean isStart();
    public abstract Optional<String> getEntryFunction();
    public abstract Optional<String> getExitFunction();
    public abstract Map<String, PEvent> getDeferedEvents();
    public abstract Map<String, PEvent> getIgnoredEvents();

    public abstract List<PTransition> getTransitions();
    public abstract Map<PEvent, PTransition> getTransitionsMap();

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