package com.multipathp.pprogram.types;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Objects;

public class PTypeTuple extends PType implements IPTypeGetIndexable {
    ArrayList<PType> Ts = new ArrayList<>();

    public PTypeTuple(Collection<PType> Ts) {
        super("$tuple$", true);
    }

    @Override
    public PType getIndexedInnerType(int index) {
        return Ts.get(index);
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof PTypeTuple)) {
            return false;
        }
        PTypeTuple other = (PTypeTuple) o;
        return Objects.equals(Ts, other.Ts);
    }

    @Override
    public int hashCode() {
        return Objects.hash(Ts, super.hashCode());
    }
}