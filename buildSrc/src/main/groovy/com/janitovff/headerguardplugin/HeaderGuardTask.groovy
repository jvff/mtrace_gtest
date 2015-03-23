package com.janitovff.headerguardplugin

import java.io.BufferedReader
import java.io.FileInputStream
import java.io.InputStream
import java.io.InputStreamReader
import java.io.IOException
import java.util.regex.Matcher
import java.util.regex.Pattern

import org.gradle.api.DefaultTask
import org.gradle.api.file.FileTree
import org.gradle.api.tasks.SourceSet
import org.gradle.api.tasks.TaskAction

import static com.janitovff.headerguardplugin.HeaderGuardException.*

public class HeaderGuardTask extends DefaultTask  {
    private HashMap<File, Exception> failures = new HashMap<File, Exception>()

    @TaskAction
    public void checkHeaders() throws HeaderGuardException {
        failures.clear()

        for (def sourceSet : project.sources) {
            for (def languageSourceSet : sourceSet)
                checkHeadersIn(languageSourceSet.source)
        }
    }

    private void checkHeadersIn(FileTree fileTree) throws HeaderGuardException {
        fileTree.visit { fileDetails ->
            if (isHeader(fileDetails.getName()))
                checkHeader(fileDetails.getFile())
        }

        checkForFailures()
    }

    private void checkForFailures() throws HeaderGuardException {
        if (!failures.isEmpty()) {
            for (File file : failures.keySet())
                println file.getPath() + ": " + failures.get(file).getMessage()

            throw FAIL
        }
    }

    public boolean isHeader(String fileName) {
        return fileName.matches(".*\\.h(pp|xx)?")
    }

    public void checkHeader(File headerFile) throws IOException {
        FileInputStream inputStream = new FileInputStream(headerFile)
        InputStreamReader fileStreamReader = new InputStreamReader(inputStream)
        BufferedReader fileReader = new BufferedReader(fileStreamReader)

        try {
            checkGuards(fileReader)
        } catch (HeaderGuardException exception) {
            failures.put(headerFile, exception)
        } finally {
            inputStream.close()
        }
    }

    private void checkGuards(BufferedReader fileReader) throws IOException,
            HeaderGuardException {
        String guard = findGuard(fileReader)

        checkGuardDefinition(guard, fileReader)
        checkGuardEnd(fileReader)
    }

    private String findGuard(BufferedReader fileReader) throws IOException,
            HeaderGuardException {
        String firstLine = skipIgnoredLines(fileReader)

        if (firstLine == null)
            throw FAIL_GUARD_NOT_FOUND

        return extractGuard(firstLine)
    }

    private String extractGuard(String line) throws IOException,
            HeaderGuardException {
        String guardCheckRegExp = "#ifndef[ \t]+([a-zA-Z_][a-zA-Z0-9_]+)"
        Pattern guardCheck = Pattern.compile(guardCheckRegExp)
        Matcher matcher = guardCheck.matcher(line.trim())

        if (!matcher.matches())
            throw FAIL_GUARD_NOT_FOUND

        return matcher.group(1)
    }

    private void checkGuardDefinition(String guard, BufferedReader fileReader)
            throws IOException, HeaderGuardException {
        String line = skipIgnoredLines(fileReader)

        if (line == null || !containsGuardDefinition(guard, line))
            throw FAIL_GUARD_NOT_DEFINED
    }

    private boolean containsGuardDefinition(String guard, String line) {
        String guardDefinitionRegExp = "#define[ \t]+" + guard
        Pattern guardDefinition = Pattern.compile(guardDefinitionRegExp)
        Matcher matcher = guardDefinition.matcher(line.trim())

        return matcher.matches()
    }

    private void checkGuardEnd(BufferedReader fileReader) throws IOException,
            HeaderGuardException {
        String lastLine = skipToLastLine(fileReader)

        if (lastLine == null)
            throw FAIL_GUARD_NOT_CLOSED

        String guardEndRegExp = "#endif"
        Pattern guardEnd = Pattern.compile(guardEndRegExp)
        Matcher matcher = guardEnd.matcher(lastLine.trim())

        if (!matcher.matches())
            throw FAIL_GUARD_NOT_CLOSED
    }

    private String skipToLastLine(BufferedReader fileReader)
            throws IOException {
        String lastLine = fileReader.readLine()
        String currentLine = lastLine

        while (currentLine != null) {
            lastLine = currentLine
            currentLine = skipIgnoredLines(fileReader)
        }

        return lastLine
    }

    private String skipIgnoredLines(BufferedReader fileReader)
            throws IOException, HeaderGuardException {
        String line = skipEmptyLines(fileReader)

        if (line == null)
            return line

        while (startsWithBlockComment(line)) {
            line = skipBlockComment(fileReader)

            if (line == null)
                return line

            if (lineIsEmpty(line))
                line = skipEmptyLines(fileReader)
        }

        return line
    }

    private String skipEmptyLines(BufferedReader fileReader)
            throws IOException {
        String line = fileReader.readLine()

        while (line != null && lineIsEmpty(line))
            line = fileReader.readLine()

        if (line != null)
            line.trim()

        return line
    }

    private boolean lineIsEmpty(String line) {
        line = removeSingleLineBlockComments(line.trim())

        return "".equals(line) || line.matches("//.*")
    }

    private String removeSingleLineBlockComments(String line) {
        while (startsWithBlockComment(line)) {
            int commentEndIndex = line.indexOf("*/")

            if (commentEndIndex < 0)
                return line
            else
                line = line.substring(commentEndIndex + 2).trim()
        }

        return line
    }

    private boolean startsWithBlockComment(String line) {
        return line.matches("/\\*.*")
    }

    private String skipBlockComment(BufferedReader fileReader)
            throws IOException, HeaderGuardException {
        String line = fileReader.readLine()

        if (line == null)
            throw FAIL_UNTERMINATED_BLOCK_COMMENT

        int commentEndIndex = line.indexOf("*/")

        while (commentEndIndex < 0) {
            line = fileReader.readLine()

            if (line == null)
                throw FAIL_UNTERMINATED_BLOCK_COMMENT

            commentEndIndex = line.indexOf("*/")
        }

        return line.substring(commentEndIndex + 2).trim()
    }
}
