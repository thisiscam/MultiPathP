package com.multipathp.pprogram.ast;

import org.inferred.freebuilder.FreeBuilder;

import java.util.Optional;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
@FreeBuilder
public abstract class ConditionalStmt extends Stmt {
    public abstract Exp getCondition();
    public abstract Stmt getThenBranch();
    public abstract Optional<Stmt> getElseBranch();

    @Override
    public int getChildrenCount() {
        return 2 + (getElseBranch().isPresent() ? 1 : 0);
    }

    @Override
    public PASTNode getChild(int i) {
        switch (i) {
            case 0: return getCondition();
            case 1: return getThenBranch();
            case 2: {
                if(getElseBranch().isPresent()) {
                    return getElseBranch().get();
                }
            }
            default: throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends ConditionalStmt_Builder {

    }
}
