package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class DefaultExp extends Exp {
    public abstract PType getTypeForDefault();

    @Override
    public PType getExpressionType() {
        return getTypeForDefault();
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

    public static class Builder extends DefaultExp_Builder {

    }
}
