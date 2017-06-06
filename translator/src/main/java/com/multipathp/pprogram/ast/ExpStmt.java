package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/30/17.
 */
@FreeBuilder
public abstract class ExpStmt extends Stmt {
    public abstract Exp getExpression();

    @Override
    public int getChildrenCount() {
        return 1;
    }

    @Override
    public PASTNode getChild(int i) {
        if(i == 0) {
            return getExpression();
        } else {
            throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends ExpStmt_Builder {

    }
}
