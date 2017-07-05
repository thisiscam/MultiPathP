package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.IPTypeGetIndexable;
import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class GetIndexExp extends Exp {
    public abstract Exp getTargetExpression();
    public abstract int getIndex();

    @Override
    public PType getExpressionType() {
        Exp targetExp = getTargetExpression();
        IPTypeGetIndexable tupleType = (IPTypeGetIndexable) targetExp.getExpressionType();
        return tupleType.getIndexedInnerType(getIndex());
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

    public static class Builder extends GetIndexExp_Builder {

    }
}
