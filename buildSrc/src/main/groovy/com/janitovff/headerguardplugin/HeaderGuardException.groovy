package com.janitovff.headerguardplugin

public class HeaderGuardException extends Exception {
    public static final FAIL =
            new HeaderGuardException("Header guard check failed")
    public static final FAIL_GUARD_NOT_FOUND =
            new HeaderGuardException("Header guard not found");
    public static final FAIL_GUARD_NOT_DEFINED =
            new HeaderGuardException("Header guard not defined");
    public static final FAIL_GUARD_NOT_CLOSED =
            new HeaderGuardException("Header guard not closed");
    public static final FAIL_UNTERMINATED_BLOCK_COMMENT =
            new HeaderGuardException("Unterminated block comment");

    public HeaderGuardException(String message) {
        super(message)
    }
}
