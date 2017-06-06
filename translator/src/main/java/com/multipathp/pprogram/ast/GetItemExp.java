package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.IPTypeGetItemable;
import com.multipathp.pprogram.types.PType;
import com.multipathp.pprogram.types.PTypeSeq;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class GetItemExp extends Exp {
    public abstract Exp getTargetExpression();
    public abstract Exp getIndexerExpression();

    @Override
    public PType getExpressionType() {
        return ((IPTypeGetItemable) getTargetExpression().getExpressionType()).getIndexedInnerType();
    }

    @Override
    public int getChildrenCount() {
        return 2;
    }

    @Override
    public PASTNode getChild(int i) {
        switch (i) {
            case 0: return getTargetExpression();
            case 1: return getIndexerExpression();
            default: throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends GetItemExp_Builder {

    }
}
