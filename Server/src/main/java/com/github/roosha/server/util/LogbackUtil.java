package com.github.roosha.server.util;

import ch.qos.logback.classic.LoggerContext;
import ch.qos.logback.classic.joran.JoranConfigurator;
import ch.qos.logback.core.joran.spi.JoranException;
import ch.qos.logback.core.util.StatusPrinter;
import lombok.val;
import org.jetbrains.annotations.Nullable;
import org.slf4j.LoggerFactory;

import java.io.File;

public class LogbackUtil {
    /**
     * Try to configure Logback using logback file placed in the filesystem. If no config file found,
     * do nothing(default Logback configuration will be used).
     * The locations will be checked out in order to find file:
     * <ol>
     *     <li>~/.roosha-logback.xml</li>
     *     <li>~/.roosha-server/logback.xml</li>
     *     <li>File, specified in system property {@code roosha.server.config.logback}</li>
     *     <li>File, specified in environment variable {@code ROOSHA_SERVER_LOGBACK}</li>
     * </ol>
     */
    public static void configureLogback() {
        val configFile = findLogbackConfigFile();
        if (configFile == null) {
            LoggerFactory.getLogger("root").info("Failed to find any configuration file for Logback " +
                    "in the file system. Use default configuration.");
            return;
        }

        // How it works --- http://logback.qos.ch/manual/configuration.html#joranDirectly
        val context = (LoggerContext) LoggerFactory.getILoggerFactory();
        try {
            val configurator = new JoranConfigurator();
            configurator.setContext(context);
            context.reset();
            configurator.doConfigure(configFile);
        } catch (JoranException e) {
            // StatusPrinter will handle it
        }
        StatusPrinter.printInCaseOfErrorsOrWarnings(context);
    }

    private static @Nullable File findLogbackConfigFile() {
        val homeDir = System.getProperty("user.home");
        File result;
        if ((result = checkFile(homeDir + "/.roosha-logback.xml")) != null) {
            return result;
        } else if ((result = checkFile(homeDir + "/.roosha-server/logback.xml")) != null) {
            return result;
        } else if ((result = checkFile(System.getProperty("roosha.server.config.logback"))) != null) {
            return result;
        } else {
            return checkFile(System.getenv("ROOSHA_SERVER_LOGBACK"));
        }
    }

    private static @Nullable File checkFile(String fileName) {
        if (fileName == null) {
            return null;
        }
        val file = new File(fileName);
        return file.exists() && file.canRead() && file.isFile() ? file : null;
    }
}
