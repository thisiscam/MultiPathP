package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import javax.annotation.Nullable;
import java.util.List;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class NewExp extends Exp {
    public abstract String getMachineIdentifier();
    @Nullable
    public abstract Exp getPayloadExpression();

    @Override
    public PType getExpressionType() {
        return PType.MACHINE;
    }

    @Override
    public int getChildrenCount() {
        return getPayloadExpression() == null ? 0 : 1;
    }

    @Override
    public PASTNode getChild(int i) {
        if(i == 0) {
            return getPayloadExpression();
        } else {
            throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends NewExp_Builder {

    }
}
