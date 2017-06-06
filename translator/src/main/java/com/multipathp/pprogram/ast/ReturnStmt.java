package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

import java.util.Optional;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class ReturnStmt extends Stmt {
    public abstract Optional<Exp> getExpression();

    @Override
    public int getChildrenCount() {
        return getExpression().isPresent() ? 1 : 0;
    }

    @Override
    public PASTNode getChild(int i) {
        if(getExpression().isPresent() && i == 0) {
            return getExpression().get();
        } else {
            throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends ReturnStmt_Builder {

    }
}
