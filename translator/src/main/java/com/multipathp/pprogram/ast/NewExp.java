package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.List;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class NewExp extends Exp {
    public abstract String getMachineIdentifier();
    public abstract List<Exp> getArguments();

    @Override
    public PType getExpressionType() {
        return PType.MACHINE;
    }

    @Override
    public int getChildrenCount() {
        return getArguments().size();
    }

    @Override
    public PASTNode getChild(int i) {
        return getArguments().get(i);
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends NewExp_Builder {

    }
}
