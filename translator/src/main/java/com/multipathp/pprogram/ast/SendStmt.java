package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

import java.util.List;

/**
 * Created by jianqiaoyang on 5/29/17.
 */

@FreeBuilder
public abstract class SendStmt extends Stmt {
    public abstract Exp getTargetExpression();
    public abstract Exp getMessageExpression();
    public abstract List<Exp> getArguments();

    @Override
    public int getChildrenCount() {
        return 2 + getArguments().size();
    }

    @Override
    public PASTNode getChild(int i) {
        if(i == 0) {
            return getTargetExpression();
        } else if (i == 1) {
            return getMessageExpression();
        } else {
            return getArguments().get(i);
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends SendStmt_Builder {

    }
}
