package com.multipathp.pprogram.types;

import java.util.Objects;

public class PTypeSeq extends PType implements IPTypeGetItemable {
    PType T;

    public PTypeSeq(PType T) {
        super("$seq$", true);
        this.T = T;
    }

    @Override
    public PType getIndexedInnerType() {
        return T;
    }

    @Override
    public boolean equals(Object o) {

        if (o == this) return true;
        if (!(o instanceof PTypeSeq)) {
            return false;
        }
        PTypeSeq other = (PTypeSeq) o;
        return Objects.equals(T, other.T);
    }

    @Override
    public int hashCode() {
        return Objects.hash(T, super.hashCode());
    }
}