package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class ArithmeticExp extends BinaryExp {
    public enum Kind {
        ADD,
        SUB,
        MUL,
        DIV
    };

    public abstract Kind getKind();

    @Override
    public PType getExpressionType() {
        return PType.INT;
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends ArithmeticExp_Builder {

    }
}
