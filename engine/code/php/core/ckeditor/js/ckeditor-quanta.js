$(document).bind('shadow_node_add',function(ev) { ckeditor(); });
$(document).bind('shadow_node_edit',function(ev) { ckeditor(); });

function ckeditor(ev) {
    CKEDITOR.replace('edit_content', {
        shiftEnterMode: CKEDITOR.ENTER_DIV,
        fillEmptyBlocks: false,
        fullPage: false,
        enterMode: CKEDITOR.ENTER_BR,
        lineBreakChars: '',
        autoParagraph: false,
        toolbar: [
            {name: 'source', items: ['Source']},
            {name: 'styles', items: ['Styles', 'Format', 'FontSize']},
            {name: 'paragraph', items: ['List', 'Indent', 'Align']},

            ['Cut', 'Copy', 'Paste', 'PasteText', 'PasteFromWord', '-', 'Undo', 'Redo'],			// Defines toolbar group without name.
            {name: 'basicstyles', items: ['Bold', 'Italic']},
            {
                name: 'links',
                items: ['Link', 'Unlink', 'Anchor', '-', 'Table', 'HorizontalRule', 'Smiley', 'SpecialChar', 'PageBreak']
            }
        ]
    });
}