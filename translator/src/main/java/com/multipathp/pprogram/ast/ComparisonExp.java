package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class ComparisonExp extends BinaryExp {
    public enum Kind {
        LT,
        LE,
        GT,
        GE,
        NE,
        EQ
    };

    public abstract Kind getKind();

    @Override
    public PType getExpressionType() {
        return PType.BOOL;
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends ComparisonExp_Builder {

    }
}
