package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

import java.util.List;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class RaiseStmt extends Stmt {
    public abstract List<Exp> getArguments();

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

    public static class Builder extends RaiseStmt_Builder {

    }
}
