package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/31/17.
 */
@FreeBuilder
public abstract class InExp extends BinaryExp {

    @Override
    public PType getExpressionType() {
        return PType.BOOL;
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends InExp_Builder {

    }
}
