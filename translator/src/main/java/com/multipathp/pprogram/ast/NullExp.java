package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class NullExp extends LiteralExp {

    @Override
    public PType getExpressionType() {
        return PType.NULL;
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends NullExp_Builder {

    }
}
