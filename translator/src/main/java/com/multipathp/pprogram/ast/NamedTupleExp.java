package com.multipathp.pprogram.ast;

import com.google.common.base.Preconditions;
import com.multipathp.pprogram.types.PType;
import com.multipathp.pprogram.types.PTypeNamedTuple;
import org.inferred.freebuilder.FreeBuilder;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class NamedTupleExp extends Exp {
    public abstract List<String> getNames();
    public abstract List<Exp> getArguments();

    public Exp getByName(String name) {
        int idx = getNames().indexOf(name);
        return getArguments().get(idx);
    }

    @Override
    public PType getExpressionType() {
        LinkedHashMap<String, PType> typeMap = new LinkedHashMap<>();
        for(int i=0; i < getNames().size(); i++) {
            typeMap.put(getNames().get(i), getArguments().get(i).getExpressionType());
        }
        return new PTypeNamedTuple(typeMap);
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

    public static class Builder extends NamedTupleExp_Builder {

        public Builder addNamedPair(String name, Exp argumentExpression) {
            return addNames(name).addArguments(argumentExpression);
        }

        @Override
        public NamedTupleExp build() {
            NamedTupleExp ret = super.build();
            Preconditions.checkState(ret.getNames().size() == ret.getArguments().size());
            return ret;
        }
    }
}
