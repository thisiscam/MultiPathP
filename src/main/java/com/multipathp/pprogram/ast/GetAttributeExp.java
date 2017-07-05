package com.multipathp.pprogram.ast;

import com.google.common.base.Preconditions;
import com.multipathp.pprogram.types.PType;
import com.multipathp.pprogram.types.PTypeNamedTuple;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class GetAttributeExp extends Exp {
    public abstract Exp getTargetExpression();
    public abstract String getAttributeName();

    public int getIndex() {
        PTypeNamedTuple nmdType =  (PTypeNamedTuple) getTargetExpression().getExpressionType();
        Preconditions.checkState(nmdType != null);
        return nmdType.getIndexByAttribute(getAttributeName());
    }

    @Override
    public PType getExpressionType() {
        Exp target = getTargetExpression();
        PTypeNamedTuple targetType = (PTypeNamedTuple) target.getExpressionType();
        return targetType.getTypeByAttribute(getAttributeName());
    }

    @Override
    public int getChildrenCount() {
        return 1;
    }

    @Override
    public PASTNode getChild(int i) {
        if(i == 0) {
            return getTargetExpression();
        } else {
            throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends GetAttributeExp_Builder {

    }
}
