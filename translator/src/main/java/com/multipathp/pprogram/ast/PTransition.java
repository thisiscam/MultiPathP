package com.multipathp.pprogram.ast;

import com.google.common.base.Preconditions;
import com.multipathp.pprogram.PEvent;
import org.inferred.freebuilder.FreeBuilder;

import java.util.Optional;

@FreeBuilder
public abstract class PTransition extends PASTNode {
    public abstract Optional<String> getFunctionName();
    public abstract PEvent getOnE();
    public abstract String getToStateName();
    public abstract boolean isPush();

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
            Preconditions.checkState(ret.getFunctionName().isPresent() ^ isPush());
            return ret;
        }
    }
}