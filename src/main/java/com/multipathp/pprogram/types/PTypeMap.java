package com.multipathp.pprogram.types;

import java.util.Objects;

public class PTypeMap extends PType implements IPTypeGetItemable {
    PType Tk, Tv;

    public PTypeMap(PType Tk, PType Tv) {
        super("$map$", true);
        this.Tk = Tk;
        this.Tv = Tv;
    }

    public PType getKeyType() {
        return Tk;
    }

    public PType getValueType() {
        return Tv;
    }

    @Override
    public PType getIndexedInnerType() {
        return getValueType();
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof PTypeMap)) {
            return false;
        }
        PTypeMap other = (PTypeMap) o;
        return Objects.equals(Tk, other.Tk) && Objects.equals(Tv, other.Tv);
    }

    @Override
    public int hashCode() {
        return Objects.hash(Tk, Tv, super.hashCode());
    }
}