package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

import java.util.List;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class AnnounceStmt extends Stmt {
    public abstract Exp getTargetExpression();
    public abstract List<Exp> getArguments();

    @Override
    public int getChildrenCount() {
        return 1 + getArguments().size();
    }

    @Override
    public PASTNode getChild(int i) {
        if(i == 0) {
            return getTargetExpression();
        } else {
            return getArguments().get(i - 1);
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends AnnounceStmt_Builder {

    }
}
