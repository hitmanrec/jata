<template>
	
	<update-item-dialog v-model:show="UpdateItemDialogVisible">
		<update-item-form 
			@add-item="addItem"
			@update-item="updateItemProvide"
			:actionType='actionType'
			:item_to_update="item_to_update_internal"
		/>
	</update-item-dialog>
	<div>
		<small-button class="add-item-button" @click="UpdateItemDialogVisible = true; actionType = 'add'">
			+ New item
		</small-button>
		<transition-group name="item-list">
			
			<simple-item class="list-item"
				v-for="item in items" 
				:item="item"
				:key="item.id"
				@update-item="updateItemRecieve($event)"
				@delete-item="$emit('delete-item', item)"
				:class="{'planned-item': item.status === 'planned', 'started-item': item.status === 'started', 'complete-item': item.status === 'complete', 'dropped-item': item.status === 'dropped'}"
			/>		
		</transition-group>
	</div>
</template>

<script>
import SimpleItem from '@/components/SimpleItem'
import UpdateItemDialog from '@/components/UI/UpdateItemDialog'
import UpdateItemForm from '@/components/UpdateItemForm'

export default {
	components: {
		SimpleItem,
		UpdateItemDialog,
		UpdateItemForm
	},
	props: {
		items: {
			type: Array,
			required: true
		},
		category: {
			type: Object,
			default: () => ({name: '', id: 0, removed: false})
		}
	},
	data() {
		return {
			UpdateItemDialogVisible: false,
			actionType: 'add',
			item_to_update_internal: {}
		}
	},
	methods: {
		addItem(item) {
			this.$emit('add-item', item)
			this.UpdateItemDialogVisible = false
		},
		updateItemProvide(item) {
			this.$emit('update-item', item)
			this.UpdateItemDialogVisible = false
		},
		updateItemRecieve(item) {
			this.item_to_update_internal = item
			this.UpdateItemDialogVisible = true
			this.actionType = 'update'
		}
	}
}
</script>

<style scoped>
.list-item{
	margin-top: 10px;
}

.add-item-button{
	margin: 10px auto;
	
}

.item-list-item{
	display: inline-block;
	margin-right: 10px;
}

.item-list-enter-active,
.item-list-leave-active{
	transition: all 0.5s ease;
}

.item-list-enter-from,
.item-list-leave-to{
	opacity: 0;
	transform: translateX(100px);

}

.item-list-move{
	transition: transform 0.5s ease;
}

.planned-item{
	border-color: var(--planned-col) !important;
}

.started-item{
	border-color: var(--started-col) !important;
}

.complete-item{
	border-color: var(--complete-col) !important;
}

.dropped-item{
	border-color: var(--dropped-col) !important;
}
</style>