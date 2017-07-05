package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class AssertStmt extends Stmt {
    abstract Exp getExp();
    abstract String getMessage();

    @Override
    public PASTNode getChild(int i) {
        if(i == 0) {
            return getExp();
        } else {
            throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public int getChildrenCount() {
        return 1;
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends AssertStmt_Builder {

    }
}
