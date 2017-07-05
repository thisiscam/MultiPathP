package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

import javax.annotation.Nullable;
import java.util.List;

/**
 * Created by jianqiaoyang on 5/29/17.
 */

@FreeBuilder
public abstract class SendStmt extends Stmt {
    public abstract Exp getTargetExpression();
    public abstract Exp getEventExpression();
    @Nullable
    public abstract Exp getPayloadExpression();

    @Override
    public int getChildrenCount() {
        return getPayloadExpression() == null ? 2 : 3;
    }

    @Override
    public PASTNode getChild(int i) {
        switch (i) {
            case 0: return getTargetExpression();
            case 1: return getEventExpression();
            case 2: return getPayloadExpression();
            default: throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends SendStmt_Builder {

    }
}
