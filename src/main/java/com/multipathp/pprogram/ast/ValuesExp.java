package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import com.multipathp.pprogram.types.PTypeMap;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class ValuesExp extends UnaryExp {

    public PType getExpressionType() {
        return ((PTypeMap) getExpression().getExpressionType()).getValueType();
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends ValuesExp_Builder {

    }
}
