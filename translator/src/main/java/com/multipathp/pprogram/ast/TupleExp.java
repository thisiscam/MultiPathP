package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import com.multipathp.pprogram.types.PTypeTuple;
import org.inferred.freebuilder.FreeBuilder;

import java.util.List;
import java.util.stream.Collectors;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class TupleExp extends Exp {
    public abstract List<Exp> getArguments();

    @Override
    public PType getExpressionType() {
        return new PTypeTuple(getArguments().stream().map(Exp::getExpressionType).collect(Collectors.toList()));
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


    public static class Builder extends TupleExp_Builder {

    }
}
