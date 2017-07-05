package com.multipathp.pprogram.ast;

import com.google.common.base.Preconditions;
import com.multipathp.pprogram.PEvent;
import org.inferred.freebuilder.FreeBuilder;

import javax.annotation.Nullable;
import java.util.Optional;

@FreeBuilder
public abstract class PTransition extends PASTNode {
    @Nullable
    public abstract String getFunctionName();
    public abstract PEvent getOnE();
    @Nullable
    public abstract String getToStateName();
    public abstract boolean isPush();

    public static PTransition HALT_TRANSITION = new PTransition.Builder()
            .setPush(false)
            .setOnE(PEvent.EVENT_HALT)
            .setToStateName(PMachineState.HALT_STATE.getName()).build();

    @Nullable
    PFunction function;
    public PFunction getFunction() {
        if(getFunctionName() != null && function == null) {
            throw new IllegalStateException("function must be set first");
        }
        return function;
    }
    public void setFunction(PFunction function) {
        if(this.function != null) {
            throw new IllegalStateException("function cannot be re-set");
        }
        this.function = function;
    }

    @Override
    public int getChildrenCount() {
        return 0;
    }

    @Override
    public PASTNode getChild(int i) {
        throw new IndexOutOfBoundsException();
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends PTransition_Builder {
        @Override
        public PTransition build() {
            PTransition ret = super.build();
            Preconditions.checkState((ret.getFunctionName() != null) || !isPush());
            return ret;
        }
    }
}