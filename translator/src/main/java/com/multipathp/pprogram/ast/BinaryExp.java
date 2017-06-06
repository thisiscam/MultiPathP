package com.multipathp.pprogram.ast;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
public abstract class BinaryExp extends Exp {
    abstract Exp getLeft();
    abstract Exp getRight();

    @Override
    public final PASTNode getChild(int i) {
        switch (i) {
            case 0: {
                return getLeft();
            }
            case 1: {
                return getRight();
            }
            default: {
                throw new IndexOutOfBoundsException();
            }
        }
    }

    @Override
    public final int getChildrenCount() {
        return 2;
    }
}
