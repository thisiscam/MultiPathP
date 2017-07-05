package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class WhileStmt extends Stmt {
    public abstract Exp getCondition();
    public abstract Stmt getStmt();

    @Override
    public int getChildrenCount() {
        return 2;
    }

    @Override
    public PASTNode getChild(int i) {
        switch (i) {
            case 0: return getCondition();
            case 1: return getStmt();
            default:throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends WhileStmt_Builder {

    }
}
