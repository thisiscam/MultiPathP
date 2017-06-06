package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class GroupExp extends Exp {
    public abstract Exp getExpression();

    @Override
    public PType getExpressionType() {
        return getExpression().getExpressionType();
    }

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

    public static class Builder extends GroupExp_Builder {

    }
}
