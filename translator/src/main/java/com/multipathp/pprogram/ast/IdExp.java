package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class IdExp extends Exp {
    public abstract String getName();

    private PType expressionType;
    @Override
    public PType getExpressionType() {
        if(expressionType == null) {
            throw new IllegalStateException("type must be annotated first");
        }
        return expressionType;
    }

    public void setExpressionType(PType type) {
        if(expressionType != null) {
            throw new IllegalStateException("type cannot be re-annotated");
        }
        expressionType = type;
    }

    @Override
    public int getChildrenCount() {
        return 0;
    }

    @Override
    public PASTNode getChild(int i) {
        throw new IndexOutOfBoundsException();
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends IdExp_Builder {

    }
}
