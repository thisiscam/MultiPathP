package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class SetIndexStmt extends Stmt {
    public abstract GetIndexExp getTarget();
    public abstract Exp getExpression();

    @Override
    public int getChildrenCount() {
        return 2;
    }

    @Override
    public PASTNode getChild(int i) {
        switch (i) {
            case 0: return getTarget();
            case 1: return getExpression();
            default: throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends SetIndexStmt_Builder {

    }
}
