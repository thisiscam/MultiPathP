package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

import java.util.List;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class StmtListStmt extends Stmt {
    public abstract List<Stmt> getStmts();

    @Override
    public int getChildrenCount() {
        return getStmts().size();
    }

    @Override
    public PASTNode getChild(int i) {
        return getStmts().get(i);
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }


    public static class Builder extends StmtListStmt_Builder {

    }
}
