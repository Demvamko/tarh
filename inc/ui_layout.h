static Tag tags[] = {
    {
        .anchors = { 0.0f, 0.0f, 1.0f, 1.0f },
        .position = POSITION_ANCHORED
    },
        {
            .anchors = { 0.0f, 0.0f, 1.0f, 1.0f },
            .position = POSITION_ANCHORED,
            .depth = 1
        },
            {
                .rect = { 0, 0, 512, 512 },
                .text_str = example,
                .text_len = sizeof(example),
                .font_size = 12,
                .position = POSITION_ABSOLUTE,
                .depth = 2,
                .index = 1,
                .visible = 1,
                .uv = { 0.0f, 0.0f, 0.5f, 0.5f },
                .color = { 255, 255, 255, 255 }
            }
};
