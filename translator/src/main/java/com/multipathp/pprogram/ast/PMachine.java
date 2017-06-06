package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.PEvent;
import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.*;

@FreeBuilder
public abstract class PMachine extends PASTNode {
    public abstract String getName();
    public abstract boolean isMain();
    public abstract boolean isSpec();
    public abstract Map<String, PEvent> getObservedEvents();
    public abstract Map<String, PType> getVarDecls();
    public abstract Map<String, PFunction> getFunDecls();
    public abstract Map<String, PMachineState> getStateDecls();

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
            return (PMachineState) getStateDecls().values().toArray()[i];
        }
        throw new IndexOutOfBoundsException();
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends PMachine_Builder {
        public Builder() {
            setSpec(false);
        }

        public Builder putFunDecls(PFunction function) {
            return putFunDecls(function.getName(), function);
        }
    }
}