package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

import javax.annotation.Nullable;
import java.util.Optional;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class ReturnStmt extends Stmt {
    @Nullable
    public abstract Exp getExpression();

    @Override
    public int getChildrenCount() {
        return getExpression() != null ? 1 : 0;
    }

    @Override
    public PASTNode getChild(int i) {
        if(getExpression() != null && i == 0) {
            return getExpression();
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
