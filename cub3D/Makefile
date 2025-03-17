NAME                = cub3D
CC                  = cc
CC_FLAGS            = -Wall \
                      -Wextra \
                      -Werror \
                      -pedantic \
                      -O3

SRC_DIR             = src
INCLUDE_DIR         = includes
INCLUDE_FILES       = app.h \
                      get_next_line.h
INCLUDES            = $(addprefix $(INCLUDE_DIR)/, $(INCLUDE_FILES))
BUILD_DIR           = build

SRCS                = $(addprefix $(SRC_DIR)/, \
                          main.c)
SRCS                += $(addprefix $(SRC_DIR)/01_convert/, \
                          checkers.c \
                          convert.c \
                          file_elements_setter.c \
                          map_checker.c \
                          map_set.c)
SRCS                += $(addprefix $(SRC_DIR)/02_load/, \
                          errors.c \
                          map_allocation.c \
                          map_process.c \
                          parse_color.c \
                          parse_cub_data.c \
                          parse_textures.c \
                          set_player_direction.c \
                          textures.c)
SRCS                += $(addprefix $(SRC_DIR)/00_aux_funtions/, \
                          aux_2.c \
                          aux.c \
                          error.c \
                          get_next_line.c \
                          utils.c)
SRCS                += $(addprefix $(SRC_DIR)/04_render/, \
                          cleanup.c \
                          draw_minimap_line.c \
                          draw_sections.c \
                          draw_textured_wall.c \
                          frame_clear.c \
                          frame_update.c \
                          raycasting.c \
                          render_3d_view.c \
                          render_column.c \
                          render_knife.c \
                          render_minimap.c)
SRCS                += $(addprefix $(SRC_DIR)/03_input/, \
                          input_handler.c \
                          process_inputs.c)

OBJS                = $(SRCS:%.c=%.o)
BUILDS              = $(addprefix $(BUILD_DIR)/, $(OBJS))

MLX42_REPO          = https://github.com/codam-coding-college/MLX42.git
MLX42_LIB_NAME      = libmlx42.a
MLX42_DIR           = MLX42
MLX42_INCLUDE_DIR   = $(MLX42_DIR)/include/MLX42
MLX42_BUILD_DIR     = $(MLX42_DIR)/build
MLX42_LIB           = $(MLX42_BUILD_DIR)/$(MLX42_LIB_NAME)
MLX42_LIBS          = -lmlx42 \
                      -ldl \
                      -lglfw \
                      -pthread \
                      -lm
MLX42_CC            = -I./$(MLX42_INCLUDE_DIR) \
                      -L./$(MLX42_BUILD_DIR) \
                      $(MLX42_LIBS)

BLUE                = \033[0;34m
MAGENTA             = \033[0;35m
GREEN               = \033[0;32m
DEFAULT             = \033[0m

define compile
	@ printf "$(MAGENTA)$< $(BLUE)->$(GREEN) $@$(DEFAULT)\n"
	@ $(CC) -c $< \
		-I./$(INCLUDE_DIR) \
		-I./$(MLX42_INCLUDE_DIR) \
		-o $@ \
		$(CC_FLAGS)
endef

define create_objects_dir
	@ mkdir -p $(dir $@)
endef

.PHONY: all clean fclean clear re

all: $(NAME)

$(NAME): $(MLX42_LIB) $(BUILDS) $(INCLUDES)
	@ $(CC) -o $(NAME) \
	  $(BUILDS) \
	  -I./$(INCLUDE_DIR) \
	  $(MLX42_CC) \
	  $(CC_FLAGS)
	@ printf "$(GREEN)$@$(DEFAULT) successfully generated\n"

$(BUILD_DIR)/%.o: %.c
	$(call create_objects_dir)
	$(call compile)

clean_cub3d:
	@ rm -rf $(BUILD_DIR)

# Added fclean_cub3d rule so that fclean can remove the build directory entirely.
fclean_cub3d: clean_cub3d

clean: clean_MLX clean_cub3d

fclean: fclean_MLX fclean_cub3d
	@ rm -rf $(NAME)

re: fclean all

$(MLX42_LIB): $(MLX42_DIR)
	@ cd $(MLX42_DIR) && \
	  sed -i 's/cmake_minimum_required (VERSION 3.18.0)/cmake_minimum_required (VERSION 3.16.0)/g' CMakeLists.txt && \
	  cmake -B build && \
	  cmake --build build -j4

$(MLX42_DIR):
	@ git clone $(MLX42_REPO) $(MLX42_DIR)

clean_MLX: $(MLX42_DIR)
	@ if [ -d $(MLX42_BUILD_DIR) ]; then \
		find $(MLX42_BUILD_DIR) -mindepth 1 ! -name '$(MLX42_LIB_NAME)' -exec rm -rf {} +; \
	  fi

fclean_MLX: $(MLX42_DIR)
	@ rm -rf $(MLX42_BUILD_DIR)

clear_MLX:
	@ rm -rf $(MLX42_DIR)

clear: clear_MLX fclean
