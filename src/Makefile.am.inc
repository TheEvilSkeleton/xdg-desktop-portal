libexec_PROGRAMS += \
	xdg-desktop-portal \
	xdg-desktop-portal-validate-icon \
	xdg-desktop-portal-rewrite-launchers \
	$(NULL)

service_in_files += \
	src/org.freedesktop.portal.Desktop.service.in \
	src/xdg-desktop-portal.service.in \
	src/xdg-desktop-portal-rewrite-launchers.service.in \
	${NULL}

dbus_service_DATA += src/org.freedesktop.portal.Desktop.service
systemduserunit_DATA += src/xdg-desktop-portal.service
systemduserunit_DATA += src/xdg-desktop-portal-rewrite-launchers.service

xdp_dbus_built_sources = src/xdp-dbus.c src/xdp-dbus.h
xdp_impl_dbus_built_sources = src/xdp-impl-dbus.c src/xdp-impl-dbus.h
xdp_background_dbus_built_sources = src/xdp-background-dbus.c src/xdp-background-dbus.h
geoclue_built_sources = src/geoclue-dbus.c src/geoclue-dbus.h
BUILT_SOURCES += $(xdp_dbus_built_sources) $(xdp_impl_dbus_built_sources) $(xdp_background_dbus_built_sources) $(geoclue_built_sources)

$(xdp_dbus_built_sources) : $(PORTAL_IFACE_FILES)
	$(AM_V_GEN) $(GDBUS_CODEGEN)                            \
		--interface-prefix org.freedesktop.portal.      \
		--c-namespace XdpDbus                           \
		--generate-c-code $(builddir)/src/xdp-dbus      \
		$^ \
		$(NULL)

$(xdp_impl_dbus_built_sources) : $(PORTAL_IMPL_IFACE_FILES)
	$(AM_V_GEN) $(GDBUS_CODEGEN)                            \
		--interface-prefix org.freedesktop.impl.portal. \
		--c-namespace XdpDbusImpl                           \
		--generate-c-code $(builddir)/src/xdp-impl-dbus \
		$^ \
		$(NULL)

$(xdp_background_dbus_built_sources) : $(BACKGROUND_MONITOR_IFACE_FILES)
	$(AM_V_GEN) $(GDBUS_CODEGEN)                                  \
		--interface-prefix org.freedesktop.background.        \
		--c-namespace XdpDbusBackground                       \
		--generate-c-code $(builddir)/src/xdp-background-dbus \
		$^ \
		$(NULL)

$(geoclue_built_sources) : src/org.freedesktop.GeoClue2.Client.xml
	$(AM_V_GEN) $(GDBUS_CODEGEN)                            \
		--interface-prefix org.freedesktop.GeoClue2. \
		--c-namespace Geoclue                        \
		--generate-c-code $(builddir)/src/geoclue-dbus \
		$^ \
		$(NULL)

EXTRA_DIST += src/org.freedesktop.GeoClue2.Client.xml

xdg_desktop_resource_files = $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/src/xdg-desktop-portal.gresource.xml)

src/xdg-desktop-resources.h: src/xdg-desktop-portal.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $< \
		--target=$@ --sourcedir=$(srcdir) --c-name _xdg_desktop --generate-header

src/xdg-desktop-resources.c: src/xdg-desktop-portal.gresource.xml $(xdg_desktop_resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $< \
		--target=$@ --sourcedir=$(srcdir) --c-name _xdg_desktop --generate-source

EXTRA_DIST += src/xdg-desktop-portal.gresource.xml

CLEANFILES += \
	$(nodist_xdg_desktop_portal_SOURCES) \
	$(NULL)

nodist_xdg_desktop_portal_SOURCES = \
	$(xdp_dbus_built_sources)		\
	$(xdp_impl_dbus_built_sources)		\
	$(xdp_background_dbus_built_sources)	\
	$(geoclue_built_sources)		\
	src/xdg-desktop-resources.c		\
	$(NULL)

xdg_desktop_portal_SOURCES = \
	src/xdg-desktop-portal.c	\
	src/file-chooser.c		\
	src/file-chooser.h		\
	src/glib-backports.c		\
	src/glib-backports.h		\
	src/open-uri.c			\
	src/open-uri.h			\
	src/print.c			\
	src/print.h			\
	src/memory-monitor.c		\
	src/memory-monitor.h		\
	src/network-monitor.c		\
	src/network-monitor.h		\
	src/power-profile-monitor.c	\
	src/power-profile-monitor.h	\
	src/proxy-resolver.c		\
	src/proxy-resolver.h		\
	src/screenshot.c		\
	src/screenshot.h		\
	src/notification.c              \
	src/notification.h              \
	src/inhibit.c                   \
	src/inhibit.h                   \
	src/device.c                    \
	src/device.h                    \
	src/account.c                   \
	src/account.h                   \
	src/realtime.c                  \
	src/realtime.h                  \
	src/dynamic-launcher.c          \
	src/dynamic-launcher.h          \
	src/request.c			\
	src/request.h			\
	src/call.c			\
	src/call.h			\
	src/documents.c                 \
	src/documents.h                 \
	src/permissions.c               \
	src/permissions.h               \
	src/email.c                     \
	src/email.h                     \
	src/settings.c			\
	src/settings.h			\
	src/session.c			\
	src/session.h			\
	src/trash.c			\
	src/trash.h			\
	src/wallpaper.c			\
	src/wallpaper.h			\
	src/xdp-utils.c			\
	src/xdp-utils.h			\
	src/background.c		\
	src/background.h		\
	src/background-monitor.c	\
	src/background-monitor.h	\
	src/gamemode.c			\
	src/gamemode.h			\
	src/secret.c			\
	src/secret.h			\
	src/flatpak-instance.c          \
	src/flatpak-instance.h          \
	src/portal-impl.h		\
	src/portal-impl.c		\
	src/global-shortcuts.h		\
	src/global-shortcuts.c		\
	$(NULL)

if HAVE_LIBSYSTEMD
xdg_desktop_portal_SOURCES += \
	src/sd-escape.c		\
	src/sd-escape.h		\
	$(NULL)
endif

if HAVE_PIPEWIRE
xdg_desktop_portal_SOURCES += \
	src/screen-cast.c		\
	src/screen-cast.h		\
	src/remote-desktop.c		\
	src/remote-desktop.h		\
	src/pipewire.c			\
	src/pipewire.h			\
	src/camera.c			\
	src/camera.h			\
	$(NULL)
endif

if HAVE_GEOCLUE
xdg_desktop_portal_SOURCES += \
	src/location.c                  \
	src/location.h                  \
	$(NULL)
endif

xdg_desktop_portal_LDADD = \
	$(BASE_LIBS) \
	$(PIPEWIRE_LIBS) \
	$(GEOCLUE_LIBS) \
	$(SYSTEMD_LIBS) \
	$(NULL)
xdg_desktop_portal_CFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-DLIBEXECDIR=\"$(libexecdir)\" \
	-DBWRAP=\"$(BWRAP)\" \
	$(BASE_CFLAGS) \
	$(PIPEWIRE_CFLAGS) \
	$(GEOCLUE_CFLAGS) \
	$(SYSTEMD_CFLAGS) \
	-I$(srcdir)/src \
	-I$(builddir)/src \
	-I$(srcdir)/document-portal \
	$(NULL)
xdg_desktop_portal_CPPFLAGS = \
	-DGETTEXT_PACKAGE=\"$(GETTEXT_PACKAGE)\"        \
	-DLOCALEDIR=\"$(localedir)\"			\
	$(NULL)

xdg_desktop_portal_validate_icon_SOURCES = src/validate-icon.c
xdg_desktop_portal_validate_icon_LDADD = $(GDK_PIXBUF_LIBS)
xdg_desktop_portal_validate_icon_CFLAGS = $(GDK_PIXBUF_CFLAGS) -DHELPER=\"$(BWRAP)\" -D_GNU_SOURCE=1

xdg_desktop_portal_rewrite_launchers_SOURCES = src/rewrite-launchers.c
nodist_xdg_desktop_portal_rewrite_launchers_SOURCES = \
	src/dynamic-launcher.h \
	src/glib-backports.c \
	src/glib-backports.h \
	$(NULL)
xdg_desktop_portal_rewrite_launchers_LDADD = $(BASE_LIBS)
xdg_desktop_portal_rewrite_launchers_CFLAGS = $(BASE_CFLAGS)
