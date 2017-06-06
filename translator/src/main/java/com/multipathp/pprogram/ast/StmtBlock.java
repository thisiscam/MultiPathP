package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.Collection;
import java.util.LinkedHashMap;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class StmtBlock extends PASTNode {
    public abstract LinkedHashMap<String, PType> getLocalVarDecls();
    public abstract StmtListStmt getStmtList();

    @Override
    public int getChildrenCount() {
        return 1;
    }

    @Override
    public PASTNode getChild(int i) {
        if(i == 0) {
            return getStmtList();
        } else {
            throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }


    public static class Builder extends StmtBlock_Builder {
        public Builder() {
            setLocalVarDecls(new LinkedHashMap<String, PType>());
        }

        public Builder addLocalVarDecls(String name, PType type) {
            getLocalVarDecls().put(name, type);
            return this;
        }

        public Builder addLocalVarDecls(Collection<String> names, PType type) {
            for(String name : names) {
                addLocalVarDecls(name, type);
            }
            return this;
        }
    }
}
