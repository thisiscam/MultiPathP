package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.List;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class CallExp extends Exp {
    public abstract String getFunctionName();
    public abstract List<Exp> getArguments();

    private PFunction callee;
    @Override
    public PType getExpressionType() {
        if(callee == null) {
            throw new IllegalStateException("callee must be set first");
        }
        return callee.getRetType();
    }

    public PFunction getCallee() {
        if(callee == null) {
            throw new IllegalStateException("callee must be set first");
        }
        return callee;
    }

    public void setCallee(PFunction callee) {
        if(callee != null) {
            throw new IllegalStateException("callee cannot be re-set");
        }
        this.callee = callee;
    }

    @Override
    public int getChildrenCount() {
        return getArguments().size();
    }

    @Override
    public PASTNode getChild(int i) {
        return getArguments().get(i);
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends CallExp_Builder {

    }
}
