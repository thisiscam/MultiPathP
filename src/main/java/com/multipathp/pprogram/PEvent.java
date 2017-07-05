package com.multipathp.pprogram;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.Objects;
import java.util.Optional;

@FreeBuilder
public abstract class PEvent {
    public static PEvent EVENT_NULL = new PEvent.Builder().setName("$EVENT_NULL$").build();
    public static PEvent EVENT_HALT = new PEvent.Builder().setName("$EVENT_HALT$").build();

    public abstract String getName();
    public abstract Optional<PType> getPayloadType();

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof PEvent)) {
            return false;
        }
        PEvent other = (PEvent) o;
        return Objects.equals(getName(), other.getName());
    }

    @Override
    public int hashCode() {
        return Objects.hash(getName());
    }

    public static class Builder extends PEvent_Builder {

    }
}