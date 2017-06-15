package com.multipathp.pprogram.ast;

import com.google.common.base.Preconditions;
import com.multipathp.pparser.pParser;
import com.multipathp.pprogram.ast.StmtBlock;
import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;

@FreeBuilder
public abstract class PFunction extends PASTNode {
    public abstract String getName();
    public abstract LinkedHashMap<String, PType> getParams();
    public abstract PType getRetType();
    public abstract StmtBlock getStmtBlock();
    public abstract boolean isModel();

    public LinkedHashMap<String, PType> getLocalDecls() {
        return getStmtBlock().getLocalVarDecls();
    }

    @Override
    public int getChildrenCount() {
        return 1;
    }

    @Override
    public PASTNode getChild(int i) {
        if (i == 0) {
            return getStmtBlock();
        } else {
            throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public boolean getCanRaiseEvent() {
        return getDecoration(CallGraphAnnotator.CanRaiseEventAnnotation.class).getCanRaiseEvent();
    }

    public static class Builder extends PFunction_Builder {

        public Builder() {
            setParams(new LinkedHashMap<>());
            setModel(false);
        }

        public Builder addParams(String name, PType type) {
            getParams().put(name, type);
            return this;
        }
    }
}