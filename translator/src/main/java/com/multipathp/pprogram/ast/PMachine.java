package com.multipathp.pprogram.ast;

import com.google.common.base.Preconditions;
import com.multipathp.pprogram.ErrorReporter;
import com.multipathp.pprogram.PEvent;
import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.*;
import java.util.stream.Collectors;

@FreeBuilder
public abstract class PMachine extends PASTNode {
    public abstract String getName();
    public abstract boolean isMain();
    public abstract boolean isSpec();
    public abstract Map<String, PEvent> getObservedEvents();
    public abstract Map<String, PType> getVarDecls();
    public abstract Map<String, PFunction> getFunDecls();
    public abstract List<PMachineState> getStateDecls();
    public abstract PMachineState getStartState();

    public Set<PFunction> getUniqueTransitions() {
        Set<PFunction> uniqueTransitions = new HashSet<>();
        getStateDecls().stream().forEach(s -> {
            uniqueTransitions.addAll(s.getTransitions().stream().map(PTransition::getFunction).collect(Collectors.toList()));
        });
        return uniqueTransitions;
    }

    @Override
    public int getChildrenCount() {
        return getFunDecls().size() + getStateDecls().size();
    }

    @Override
    public PASTNode getChild(int i) {
        if(0 <= i && i < getFunDecls().size()) {
            return (PFunction) getFunDecls().values().toArray()[i];
        }
        i -= getFunDecls().size();
        if(0 <= i && i < getStateDecls().size()) {
            return (PMachineState) getStateDecls().toArray()[i];
        }
        throw new IndexOutOfBoundsException();
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends PMachine_Builder {
        public Builder() {
            super();
            setSpec(false);
            addStateDecls(PMachineState.HALT_STATE);
        }

        public Builder putFunDecls(PFunction function) {
            return putFunDecls(function.getName(), function);
        }

        @Override
        public PMachine build() {
            Optional<PMachineState> startState = getStateDecls().stream().filter(PMachineState::isStart).findFirst();
            Preconditions.checkArgument(startState.isPresent());
            setStartState(startState.get());
            return super.build();
        }
    }
}