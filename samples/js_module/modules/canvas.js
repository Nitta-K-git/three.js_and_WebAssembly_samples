function create(id, parent, width, height) {
    let divWrapper = document.createElement('div');
    let canvasElem = document.createElement('canvas');
    parent.appendChild(divWrapper); // div要素をparentの下に作成
    divWrapper.appendChild(canvasElem); // div要素の下にcanvas作成

    divWrapper.id = id;
    canvasElem.width = width;
    canvasElem.height = height;

    let ctx = canvasElem.getContext('2d'); // 2dレンダリングコンテキストを取得

    return {
        ctx: ctx,
        id: id
    };
}

function createReportList(wrapperId) {
    let list = document.createElement('ul');
    list.id = wrapperId + '-reporter';

    let canvasWrapper = document.getElementById(wrapperId);
    canvasWrapper.appendChild(list); // ul要素をwrapperId要素に追加

    return list.id;
}

export { create, createReportList };